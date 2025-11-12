# MIPS Pipeline Simulator

This project is a **MIPS Pipeline Simulator** written in **C++**. It simulates the execution of MIPS instructions through a 5-stage pipeline (IF, ID, EX, MEM, WB) including hazard detection, forwarding, and basic ALU operations.

---

## Features

- **Pipeline Stages:**
  - Instruction Fetch (IF)
  - Instruction Decode (ID)
  - Execute (EX)
  - Memory Access (MEM)
  - Write Back (WB)

- **Hazard Handling:**
  - Data hazards with stalling
  - Forwarding to prevent stalls

- **ALU Operations:**
  - Arithmetic: add, sub, addi, addiu
  - Logic: and, or, nor, andi, ori
  - Shift: sll, srl
  - Comparison: slt, slti
  - LUI instruction support

- **Memory:**
  - Data memory and instruction memory simulated using `unordered_map` and `vector`
  - Maintains order of memory writes for output

- **Registers:**
  - Full MIPS register file (32 registers)
  - Pre-initialized `sp` and `gp` registers

- **Control Unit:**
  - Generates control signals based on instruction type
  - Supports load/store, arithmetic, branch, and logical instructions

- **Timing:**
  - Execution time measured using a high-resolution timer

---

## Project Structure

- **Classes:**
  - `MemoryRegister` – Stores values and control signals for WB stage
  - `ExecuteRegister` – Stores EX/MEM stage values and control signals
  - `InstructionRegister` – Keeps track of instructions and program counter
  - `DataRegister` – Stores ID/EX stage data and signals
  - `Memory` – Simulates instruction and data memory
  - `Timer` – Measures execution time
  - `AluControl` – Generates ALU operation codes
  - `Alu` – Performs arithmetic and logic operations
  - `RegisterFile` – Simulates MIPS register file
  - `HazardUnit` – Detects and manages pipeline hazards
  - `ForwardingUnit` – Controls data forwarding
  - `ControlUnit` – Generates control signals based on instruction type

- **Functions:**
  - `begOutput()` – Creates initial output file with student info
  - `finalOutput()` – Writes the final state of registers, memory, cycles, and execution time

---

