package com.jupitertechnologies.jupiter;

import org.libsdl.app.SDLActivity;

/** The engine's launcher activity. SDL is linked statically into libmain.so, so the default
 *  {"SDL3", "main"} list would fail on a libSDL3.so that does not exist. */
public class JupiterActivity extends SDLActivity {
    @Override
    protected String[] getLibraries() {
        return new String[] { "main" };
    }
}
