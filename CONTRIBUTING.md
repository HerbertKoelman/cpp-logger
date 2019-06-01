# How to contribute

I'm really glad you're reading this, because we need volunteer developers to help this project come to fruition.

Here are some important resources:

  * [Our roadmap](https://github.com/HerbertKoelman/cpp-logger/milestones) is the 10k foot view of where we're going, and
  * Bugs? [GitHub cpp-logger issues](https://github.com/HerbertKoelman/cpp-logger/issues) is where to report them

## Coding conventions

Start reading our code and you'll get the hang of it. We optimize for readability:

  * We indent using four spaces (soft tabs)
  * We use [snake case](https://en.wikipedia.org/wiki/Snake_case) naming convention.
  * We indent using K&R and Stroustrup variant (more [here](https://en.m.wikipedia.org/wiki/Indentation_style)).
  * This is open source software. Consider the people who will read your code, and make it look nice for them. It's sort 
    of like driving a car: Perhaps you love doing donuts when you're alone, but with passengers the goal is to make the 
    ride as smooth as possible.
  * When it comes to UML, we use PlantUML. You can store your diagrtams in `./diagrams/`, PlantUML files should use the 
    extension `.puml` 

## Testing

We use [GoogleTest](https://github.com/google/googletest) to write and run unit tests. The cmake module `cmake/GTestExtConfig.cmake` 
upload google test source code by cloning the GitHub's repository. The CMake flag `BUILD_TESTS` can be used to turn google testing off.

Test programs are stored in `./tests/` and are setup through `./tests/CMakeLists.txt`

## Submitting changes

Please send a [GitHub Pull Request to cpp-logger](https://github.com/HerbertKoelman/cpp-logger/pulls) with a clear list 
of what you've done (read more about [pull requests](http://help.github.com/pull-requests/)). When you send a pull request, 
we will love you forever if you include Doxygen examples. We can always use more test coverage. Please follow our coding conventions 
(below) and make sure all of your commits are atomic (one feature per commit).

Always write a clear log message for your commits. One-line messages are fine for small changes, but bigger changes should look like this:

    $ git commit -m "A brief summary of the commit
    > 
    > A paragraph describing what changed and its impact."

## Continious integration

We use [Travis-CI](https://travis-ci.org/HerbertKoelman/cpp-logger/) to build and run unit tests.

Thanks,

Herbert Koelman