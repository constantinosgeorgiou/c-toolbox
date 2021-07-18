# :toolbox: C Tool Box

![Unit Tests](https://github.com/constantinosgeorgiou/c-toolbox/actions/workflows/run-tests.yml/badge.svg)

Various modules to bootstrap a C project.



Modules inspired by the document "[*Modular C: How to Write Reusable and Maintainable Code using the C Language*](http://metamodular.com/Computing/modular-c.pdf)" by Robert Strandh.


## What's included
| Module            |     Available      |
| ----------------- | :----------------: |
| Vector            | :white_check_mark: |
| List              | :white_check_mark: |
| BidirectionalList | :white_check_mark: |
| Map               | :white_check_mark: |
| Set               |     (planned)      |
| OrderedSet        |     (planned)      |
| PriorityQueue     |     (planned)      |
| Queue             |     (planned)      |
| Stack             |     (planned)      |


## Getting started

### Prerequisites
In order for everything to function smoothly you need the following software installed:
- valgrind (at least version: 3.17.0)
- gcc (at least version: 10.2.0)
- GNU Make (at least version: 4.3)

### Installing
Clone the directory, `cd` into it, and run:
```shell
make setup
```

## Running the tests

### Birds eye view
From the root of the directory you have a birds eye view to quickly see that everything is working as expected.

Available recipes:
- `make setup` is used to set up the directory for testing.
- `make tests` is used to compile all the tests of the modules.
- `make run-tests` is used to run all the tests.
- `make valgrind-tests` is used to check for memory leaks on all the tests.
- `make clean` is used to clean up all the generated files.

### Independently test a module
From the `tests/` directory you have a more fine-tuned control of each test.
```bash
# Change into the tests/ direcotry:
cd tests
```
Available general recipes:
- `make all` is used to compile all the tests.
- `make run` is used to execute all the tests.
- `make valgrind` is used to check all the tests for memory leaks.
- `make clean` is used to remove all the generated files.

Available test recipes:
- `make <test>` is used to compile `<test>`.
- `make run-<test>` is used to execute `<test>`.
- `make valgrind-<test>` is used to check `<test>` for memory leaks.
> **Note:** Replace `<test>` with the name of the desired test.


## Using a module
In order to use a module, you need to copy three things:
1. The interface file (`.h`) from `include/` directory.
2. The desired implementation (`.c`) from `modules/` directory.
3. Any dependencies of the module based on `tests/Makefile`.

> **Note:** Check an [example]() for further information.

## License
This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Acknowledgements
- Data Structures and Programming Techniques course at the University of Athens, Greece.
