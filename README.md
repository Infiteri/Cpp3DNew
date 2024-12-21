# WELCOME TO CORE

This is a fun little project started for fun. The point of this project would be to hopefully get better at programming all around as I started going to collage. This project was started in the summer of 2024, in my first year of collage and I hope i can extend and grow this project more.

# REQUIREMENTS
 - Windows32
 - g++ or other compilers
 - Patience

# CHANGELOG

Most of the (big) changes that happen to Core are written in a nice '.md' file in [CHANGELOG.md](CHANGELOG.md). This also comes with the versioning scheme of Core which is also explained a bit in [Base.h](Engine/Source/Base.h) and it works this way:

#### Version MAJOR.MINOR.SUBTLE - DD/MM/YYYY

Where: 
- MAJOR - big changes (comes in small changes but add up to a big change) 
- MINOR - small update and features, most common 
- SUBTLE - extremely small changes and bug fixes
- DD/MM/YYYY - the day, month and year when the update is pushed to git

# KNOWN PROBLEMS AND TODO'S

- Refactor the way meshes are rendered so that the renderer does the rendering and maybe batching them?
- Having no camera component crashes
- Create collider components for better structure