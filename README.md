This repo designed for koolerkx/learn-directx-3d

## Add this repo to project

1. Add all file to project
    1. Just download the zip file and copy or drag all the file into solution explorer
    2. or use git submodule `git submodule add git@github.com:koolerkx/directx3d-imgui-debugui.git DirectX3D/imgui`

2. Initialize and set the call back
    1. Call `DebugImGui_Initialize` and `DebugImGui_Finalize` at start and the end
    2. Call `DebugImGui_Update` and `DebugImGui_Draw` in loop, update should before draw
    3. Add `DebugImGui_WndProcHandler` to `WndProc` when create the window through WIN32 API
    4. Set the callback (e.g. Camera)
    
    - for 1-3 -> https://github.com/koolerkx/learn-directx-3d/commit/455b4838b74f96b63d256e8e6bfb7d35db5a344a
    - for 4 -> https://github.com/koolerkx/learn-directx-3d/commit/7357bacd1d0cccf8569ff6c11523185360880816