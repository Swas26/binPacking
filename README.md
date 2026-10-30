
The ILP algorithm requires the Homebrew HiGHS software.

To install HiGHS, run the command below:

```bash
brew install highs
```

You must specify your `BREW_DIRECTORY` in the Makefile, depending on your computer architecture or brew installation.

## Running the benchmark

```bash
make test && ./test   # sanity suite
make run              # every set in Data/bpplib, writes results/<set>.csv
```

`make run` runs each set as its own `./binpack` process, `JOBS` at a time (defaults to your number of performance cores). Options:

- `TIME_LIMIT`: ILP time limit per instance in seconds (default 10), e.g. `make run TIME_LIMIT=60`. To change the default, edit `TIME_LIMIT ?= 10` in the Makefile.
- `SETS`: only run some sets, e.g. `make run SETS="Falkenauer_T Hard28"`
- `JOBS`: how many sets run at once, e.g. `make run JOBS=2`

A single set can also be run directly: `./binpack 10 Falkenauer_T` writes `results/Falkenauer_T.csv`.

