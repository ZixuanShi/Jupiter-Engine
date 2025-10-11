// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.MemoryPool;

import jpt.Atomic;
import jpt.Mutex;
import jpt.TypeDefs;

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

        void* New();
        void Delete(void* ptr);

    private:
        bool AllocateChunk();
        void* Align(void* ptr) noexcept;
    };
}