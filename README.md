# 2048_game

A program for 2048 game that uses OOP principles and starts with a random state of two non-empty tiles (each 2 or 4 value randomly chosen) , and generates different states by applying moves to this state and the new ones until the sum of all non-empty tile values is exactly 8. If we get values such as 6 and 10, skipping 8 in between, then we backtrack, until we find a path from the initial state to an exact-8-sum state.
