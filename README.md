# Operating Systems Course Assignments

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](./LICENSE)

This repository contains my solutions for the assignments in the "Operating Systems" course at university. Each directory corresponds to a different lab.

## Project Structure

All labs organized as follows

``` graphql
.
├── CMakeLists.txt
├── README.md
└── lab[0-13]
    ├── README.md
    └── src
        └── ...
```

## Build

To build specific/all labs you need to use CMake build system with commands as follows

```bash
# Clone repository
git clone https://github.com/harakki/uni-os.git
# Go to repository root
cd uni-os

# Configure CMake
cmake -B build

# Build all labs
cmake --build build --config Release --parallel
# --- OR ---
# Build specific lab
cmake --build build --config Release --target lab[1-13]
```

## License

See the [LICENSE](./LICENSE) file for license rights and limitations (MIT).
