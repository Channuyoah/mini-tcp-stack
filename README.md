# Mini TCP Stack

A minimal user-space TCP finite state machine implementation written in C++ for learning TCP protocol internals.

This project models TCP behavior at the protocol level without using the operating system's TCP stack. It focuses on understanding how TCP state transitions work internally rather than performing real network communication.

---

## 📌 Project Purpose

The goal of this project is to deeply understand:

- TCP three-way handshake
- TCP four-way termination
- TCP state transitions
- Transmission Control Block (TCB) modeling
- Protocol-level packet abstraction
- Differences between theoretical TCP and real-world kernel implementation

This project does **not** implement a real TCP stack or raw socket communication.  
Instead, it simulates TCP behavior entirely in user space.

---

## 🧠 Current Version

**Version:** `v0.1-linear-handshake`

### Implemented

- TCP state enumeration
- Basic TCP control block (TCB)
- Three-way handshake simulation
- Four-way termination simulation
- Linear execution flow

### Limitations

- No unreliable network simulation
- No packet loss handling
- No retransmission logic
- No timeout mechanism
- No sliding window
- No congestion control
- No event-driven architecture

This version assumes a perfect network environment.

---

## 🏗 Architecture


Client TCP <---- tcp_packet_t ----> Server TCP


### Core Modules

- `state`  
  Defines TCP state enumeration and state string mapping.

- `packet`  
  Defines simplified TCP packet abstraction (SYN, ACK, FIN, SEQ).

- `tcp`  
  Implements the TCP finite state machine logic.

- `main`  
  Drives the simulation flow.

---

## 📂 Project Structure


mini-tcp-stack/
│
├── Makefile
├── README.md
├── LICENSE
│
├── main.cpp
│
└── tcp/
├── state.h
├── state.cpp
├── packet.h
├── packet.cpp
├── tcp.h
└── tcp.cpp


---

## ⚙️ Build


make
./build/tcp_sim


Clean build:


make clean


---

## 🚀 Roadmap

### v0.2
- Introduce unreliable network simulation layer
- Simulate packet loss
- Add retransmission mechanism
- Implement timeout handling

### v0.3
- Event-driven TCP simulation loop
- Basic sliding window modeling

### v1.0
- Build a simple chat system on top of the custom TCP
- Compare with real-world TCP socket implementation

---

## 📚 Learning References

- RFC 793 – Transmission Control Protocol
- TCP/IP protocol design principles
- Finite State Machine modeling in network protocols

---

## 🛠 Why This Project?

Understanding TCP at the implementation level helps develop strong intuition about:

- Reliable transport protocol design
- State machine driven systems
- Timeout and retransmission strategies
- Network protocol robustness
- Differences between theory and kernel implementation

---

## 📄 License

MIT License
