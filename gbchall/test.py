#!/usr/bin/env python3

from z3 import *

solver = Solver()

lcg_data = "1c57081b54ff00030c27786b44cf7053fcf7e8bb349fe0a3ecc7580b246f50f3"

lcg_bytes = bytes.fromhex(lcg_data)

a = BitVec("a", 8)
c = BitVec("c", 8)
seed_states = [BitVec(f"seed{idx}", 8) for idx in range(32)]

for idx in range(1, 32):
    solver.add(seed_states[idx] == (a * seed_states[idx - 1] + c))
    solver.add(seed_states[idx] == lcg_bytes[idx - 1])

while solver.check() == sat:
    model = solver.model()
    actual_seed = model[seed_states[0]]
    actual_a = model[a]
    actual_c = model[c]

    print(f"{actual_seed.as_long():08b}")
    print(f"{actual_seed=:}")
    print(f"{actual_a=:}")
    print(f"{actual_c=:}")

    solver.add(a != model[a])
    solver.add(c != model[c])
    solver.add(actual_seed != model[seed_states[0]])
