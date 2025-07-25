#ifndef APPSTATE_H
#define APPSTATE_H

// Enum for the overall application state
enum class AppState {
    StartMenu,
    AwaitingSources,
    Running,
    Paused
};

// Enum for the current interaction mode
enum class Mode {
    AddParticle,
    AddSource
};

#endif