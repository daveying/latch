# Latch

This project is called Latch because latch is the most fundamental element of electrical computer, it forms 1 single bit. the project is aimed to create a logic gate simulation framework, and create a 8-bit CPU simulation using this framework.

## TODO List
- [x] Gate template
- [x] Basic gates (NOT, OR, AND, NOR, NAND) base on GateTemplate
- [x] SR Latch using basic gates
- [x] Simple scheduler and interface
- [x] Timer
- [x] D flip flop using basic gates
- [x] Change sched::addEvent interface so that useful debug info can be logged when scheduling
    - [x] change Event to include event name and log handle
- [ ] Fix DelayedNOTGate gate::gate_info<>
- [ ] Fix gate::gate_info<>
- [ ] Simulation context
    - [ ] Gate factory
    - [ ] Fix scheduler singleton issue
    - [ ] Fix D flip flop test (Pulse)
