// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <new>

export module jpt.MemoryPool;

import jpt.Atomic;
import jpt.Mutex;
import jpt.LockGuard;
import jpt.TypeDefs;
import jpt.Math;

export namespace jpt
{
    /** A memory pool that allocates memory in chunks and provides blocks of memory from those chunks.

        @examples
            // For small objects
            MemoryPool::Config smallConfig
            {
                .blockSize = 32,
                .alignment = 8
            };
            MemoryPool smallPool(smallConfig);
            
            // For large objects
            MemoryPool::Config largeConfig
            {
                .blockSize = 256,
                .alignment = 16
            };

            MemoryPool largePool(largeConfig);    */
    class MemoryPool
    {
    public:
        struct Config
        {
            size_t blockSize = 256;            /**< Size of each memory block */
            size_t blocksPerChunk = 1024;    /**< Number of blocks per chunk */
            size_t initialChunks = 1;        /**< Number of chunks to allocate initially */
            size_t maxChunks = 8;            /**< Maximum number of chunks to allocate */
            size_t alignment = 8;            /**< Alignment of each memory block */
        };

    private:
        struct Block
        {
            Block* pNext = nullptr;
            uint8 data[1];  // Flexible array member
        };

        struct Chunk
        {
            Chunk* pNext   = nullptr;
            Block* pBlocks = nullptr;
            size_t numBlocks = 0;
        };

        Mutex m_mutex;
        Config m_config;
        Chunk* m_pChunks = nullptr;
        Block* m_pFreeList = nullptr;
        Atomic<size_t> m_numChunks = 0;
        size_t m_realBlockSize = 0;

    public:
        MemoryPool(const Config& config) noexcept;
        ~MemoryPool() noexcept;

        void* Allocate();
        void Deallocate(void* ptr);

    private:
        bool AllocateChunk();
        void* Align(void* ptr) noexcept;
    };

    MemoryPool::MemoryPool(const Config& config) noexcept
        : m_config(config)
    {
        // Ensure block size is large enough to hold a pointer
        JPT_ASSERT(config.blockSize >= sizeof(Block*));

        // Ensure alignment is a power of 2
        JPT_ASSERT(IsPowerOfTwo(config.alignment));

        // Calculate the real block size
        m_realBlockSize = sizeof(Block) + config.blockSize - 1;
        m_realBlockSize = (m_realBlockSize + config.alignment - 1) & ~(config.alignment - 1);

        // Allocate initial chunks
        for (size_t i = 0; i < config.initialChunks; ++i)
        {
            AllocateChunk();
        }
    }

    MemoryPool::~MemoryPool() noexcept
    {
        // Deallocate all chunks
        Chunk* pChunk = m_pChunks;
        while (pChunk)
        {
            Chunk* pNext = pChunk->pNext;
            ::operator delete(pChunk);
            pChunk = pNext;
        }
    }

    void* MemoryPool::Allocate()
    {
        LockGuard lock(m_mutex);

        // If no free blocks, allocate a new chunk
        if (!m_pFreeList)
        {
            if (!AllocateChunk())
            {
                return nullptr;
            }
        }

        // Get a free block from the free list
        Block* pBlock = m_pFreeList;
        m_pFreeList = pBlock->pNext;

        // Return the block data
        return Align(pBlock->data);
    }

    void MemoryPool::Deallocate(void* ptr)
    {
        if (!ptr)
        {
            return;
        }

        LockGuard lock(m_mutex);

        // Get the block from the pointer
        Block* pBlock = reinterpret_cast<Block*>(
            reinterpret_cast<uint8*>(ptr) - JPT_OFFSET_OF(Block, data)
            );

        // Add the block to the free list
        pBlock->pNext = m_pFreeList;
        m_pFreeList = pBlock;
    }

    bool MemoryPool::AllocateChunk()
    {
        // Check if we've reached the maximum number of chunks
        if (m_config.maxChunks > 0 && m_numChunks >= m_config.maxChunks)
        {
            return false;
        }

        // Allocate chunk with alignment
        size_t chunkSize = sizeof(Chunk) + m_config.blocksPerChunk * m_realBlockSize;
        void* pMemory = ::operator new(chunkSize, std::align_val_t{ m_config.alignment });
        if (!pMemory)
        {
            return false;
        }

        // Initialize chunk
        Chunk* pChunk = new(pMemory) Chunk;
        pChunk->pNext = m_pChunks;
        pChunk->numBlocks = m_config.blocksPerChunk;
        m_pChunks = pChunk;

        // Initialize free list
        uint8* pBlockMemory = reinterpret_cast<uint8*>(pChunk) + sizeof(Chunk);
        for (size_t i = 0; i < m_config.blocksPerChunk; ++i)
        {
            Block* pBlock = reinterpret_cast<Block*>(pBlockMemory + i * m_realBlockSize);
            pBlock->pNext = m_pFreeList;
            m_pFreeList = pBlock;
        }

        // Increment number of chunks
        ++m_numChunks;
        return true;
    }

    void* MemoryPool::Align(void* ptr) noexcept
    {
        uintptr address = reinterpret_cast<uintptr>(ptr);
        return reinterpret_cast<void*>((address + m_config.alignment - 1) & ~(m_config.alignment - 1));
    }
}