# Featuritis (terrible name and needs to change)

![Master - Release](https://github.com/despairblue/featuritis/workflows/Master%20-%20Release/badge.svg)

## Download

Check out the [latest master workflow] run. It has artifacts for Windows, Linux and Mac OS attached.

[latest master workflow]: https://github.com/despairblue/featuritis/actions?query=workflow%3A%22Master+-+Release%22

## Local Setup

You need Esy, you can install the beta using [npm](https://npmjs.com):

    % npm install -g esy@latest

> NOTE: Make sure `esy --version` returns at least `0.5.4` for this project to build.

Then run the `esy` command from this project root to install and build dependencies.

    % esy

## Development Flow

1. Open 2 Terminals
2. Terminal 1

   ```shell
   esy dev:watch
   ```

3. Terminal 2

   - Fish

     ```shell
     while true; esy install && esy x App || sleep 1; end
     ```

   - Bash

     ```shell
     while true; do esy install && esy x App || sleep 1; done
     ```

4. Open any editor, change and save code.
5. When ever you want to reload the app. Just close it. It will open again.
