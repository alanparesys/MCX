# MCX Tests

MCX currently includes a very small C++ test binary intended for smoke testing the core types and wiring.

## Building tests

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

This will build both the main `mcx` executable and the `mcx_tests` binary.

## Running tests

From the repository root:

```bash
./build/mcx_tests
```

At this stage, `mcx_tests` simply:

- creates a `Server` instance,
- builds a single `CHAT` event,
- passes it to `HandleEvent`,
- prints the number of actions returned.

As the project grows, this can be expanded into more structured tests or migrated to a lightweight test framework.
