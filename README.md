# Engine Y 
SDL/OpenGL Game Engine for learning purposes
ImGui windows, menus, etc...

## Built with the help of Progrematic's youtube series
 - [Youtube](https://www.youtube.com/@Progrematic)
 - [GitHub](https://github.com/progrematic)
 - His Game Engine series is easily one of the est tutorials for beginner game devs there is

## To Build
 - Clone Repo
 - From root directory
    - python3 ./cli.py gensln
    - python3 ./cli.py buildsln (see build flags for more)
    - python3 ./cli.py run
 - The Engine builds as a 
### Build Flags
 - buildsln
    - -config=[debug] || [release] 
 - run 
    - -config=[debug] || [release]
    - -proj=[MachyDev] || [PongV1] 
        - for running your own from this flag you will need to edit the run.py file located in the 'tools' directory.
### Engine
 - Machine Y Core Class (singleton)

### Machine Y
 - Main Editor for the Machine Y Engine
 - App used for testing and development

## Pong V1
 - First game built with the Machine Y Game Engine
 - Great example project