#!/bin/bash

# Colors for terminal output
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Print header
echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}       ULTRA FAST OS SIMULATION DEMONSTRATION         ${NC}"
echo -e "${CYAN}======================================================${NC}"
echo

# Check if the executable exists, if not, compile it
if [ ! -f "./fast_os_simulation" ]; then
    echo -e "${YELLOW}Compiling the OS simulation...${NC}"
    make
    echo
fi

# Run the simulation
echo -e "${GREEN}Starting the OS simulation...${NC}"
echo -e "${YELLOW}(The simulation will demonstrate a fast-booting OS with < 1s boot time)${NC}"
echo
./fast_os_simulation

# Print footer
echo
echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}                SIMULATION COMPLETE                   ${NC}"
echo -e "${CYAN}======================================================${NC}"
echo
echo -e "${GREEN}This simulation demonstrates the key concepts of a high-speed OS:${NC}"
echo -e "  - Microkernel architecture for minimal boot time"
echo -e "  - Optimized memory management with zero-copy techniques"
echo -e "  - Priority-based process scheduling for responsiveness"
echo -e "  - Parallel task execution for maximum performance"
echo -e "  - Modular design for developer-friendly architecture"
echo
echo -e "${YELLOW}For more information, see the README.md file.${NC}"