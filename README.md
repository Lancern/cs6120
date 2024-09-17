# cs6120

This repository contains my personal implementation for the programming tasks
in Cornell [CS6120]: Advanced Compilers.

[CS6120]: https://www.cs.cornell.edu/courses/cs6120

My implementation is written in Python since it's easy and convenient for a
quick start, and it allows me to focus on the most important stuff.

The implementation task of each lesson is placed under the subdirectory
corresponding to the lesson. Each lesson subdirectory is a Python package that
can be directly executed for testing.

The `bril_utils` subdirectory contains a Python package that implements some
common Bril utilities that are used across multiple lessons.

The `bril` subdirectory is a git submodule that tracks the upstream
`sampsyo/bril` repository. Make sure you have initialized and updated it after
cloning the repo:

```bash
git submodule init
git submodule update
```

> [!NOTE]
> I'm still working on this course.

## Lessons and Topics

- Lesson 2: representing programs
- Lesson 3: Local Value Numbering (LVN) & Dead Code Elimination (DCE)

## License

This work is open-source under the [MIT License](./LICENSE).
