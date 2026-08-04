// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.InputTests;

export namespace jpt
{
    /** Runs at startup outside Release. Synthetic key and mouse events need assistive access,
        so everything below the OS is asserted rather than clicked. */
    void RunInputTests();
}
