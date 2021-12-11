# Latch ![build & test](https://github.com/daveying/latch/actions/workflows/cmake.yml/badge.svg)

This project is called Latch because latch is the most fundamental element of electrical computer, it registers one single bit. the project is aimed to create a logic gate and logic circuit simulation framework, and create a 8-bit CPU simulation based on this framework.

## TODO List
- [x] Gate template
- [x] Basic gates (NOT, OR, AND, NOR, NAND) base on GateTemplate
- [x] SR Latch using basic gates
- [x] Simple scheduler and interface
- [x] Timer
- [x] D flip flop using basic gates
- [x] Change sched::addEvent interface so that useful debug info can be logged when scheduling
    - [x] change Event to include event name and log handle
- [x] Fix failing test
- [x] Component template
- [ ] Logger to support loglevel and switch on/off sched logs
    - [ ] Loglevel support
    - [ ] Schedule log can be redirect to specified file
- [ ] Simulation context
    - [ ] Gate factory
    - [ ] Fix scheduler singleton issue
    - [x] Fix D flip flop test (Pulse)
- [ ] Tool to rendering the gate
    - [ ] Option 1: rendering at runtime
    - [ ] Option 2: rendering based on logs
- [x] Fix DelayedNOTGate gate::gate_info<>
- [x] Fix gate::gate_info<>
- [ ] Name of pins are computed on the fly which saves memory but it's expected to slow down the simulation a lot
    - Schedule log as binary format
    - Tool to plot pin voltage sequence
- [x] Remove gate namespace
- [ ] Get pin instance using pin name
- [ ] Support floating state, pin connections should be non-directional.
