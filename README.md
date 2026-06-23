# Huntech: Hunter Squad Management System

This repository contains a robust, custom-built data structures project developed in C++17. Created for the Technion's Data Structures course (234218) of Winter 2026. this system simulates an association tracking "Hunters" and their squads. It manages complex attributes like Nen abilities, Aura levels, and battle experience while strictly avoiding standard library (STL) containers.

## Core Data Structures

To meet strict $O(1)$ and $O(\log k)$ time complexities, the system is built entirely from scratch using three primary data structures:

*   **Rank AVL Tree (`AuraTree`)**: A self-balancing AVL tree that holds `(squadId, Aura)` pairs, sorted primarily by Aura and secondarily by ID. It includes a custom `childrenCount` field to support $O(\log k)$ rank-based queries for finding a squad by its collective aura.
*   **Dynamic Hash Table**: A custom chaining-based hash table implemented to map Hunter IDs to their respective nodes at the following Union-Find. It operates with a maximum load factor of 1 and a growth factor of 2, dynamically rehashing to ensure amortized $O(1)$ search and insertion times.
*   **Optimized Union-Find**: A highly specialized Union-Find structure used to manage squad formations and merges using Reverse Trees. 
    *   It implements **Path Compression** and **Union by Size** to maintain $O(\log^* n)$ amortized time complexity for tracking operations. 
    *   Instead of a standard array, it utilizes a secondary AVL Tree (`groups`) to allow $O(\log k)$ access to specific squads by their ID.

## Features & API

The system provides a comprehensive C++ API for managing the Hunter Association:

*   **Squad & Hunter Registration**: Register new squads or assign individual Hunters to squads with their specific Nen ability (Manipulator, Emitter, Conjurer, Transmuter, Enhancer, or Specialist) and Aura capacity.
*   **Squad Duels**: Simulate battles between two squads. The victor is determined by their effective battle aura or, in the event of a tie, their non-transitive Nen advantages. Winning increases a squad's experience by 3, while a tie increases both by 1.
*   **Force Join**: A powerful squad can force a weaker squad to merge with it if its combined Experience, total Aura, and effective Nen ability are strictly greater than the target squad's stats.
*   **Historical Tracking**: Accurately calculates the total number of fights an individual Hunter has participated in, as well as their partial Nen ability, even across complex squad merges. This is achieved using mathematical adjustments to relative values (`relFights` and `relNen`) during path compression.

## Complexity Breakdown

*   **Space Complexity**: The entire system is strictly bounded by $O(n + k)$ space, where $n$ is the total number of Hunters ever added and $k$ is the total number of squads created.
*   **Time Complexity**:
    *   `add_squad` / `remove_squad`: $O(\log k)$.
    *   `add_hunter`: Amortized $O(\log k)$.
    *   `squad_duel`: $O(\log k)$.
    *   `force_join`: Amortized $O(\log^* n + \log k)$.
    *   `get_hunter_fights_number`: Amortized $O(\log^* n)$.
