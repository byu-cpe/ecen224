---
title: Coding Standard
layout: page
icon: fa-duotone fa-terminal
---

We decided to keep the coding standard simple so you can worry less about the standard and more about learning. The coding standard has the following requirements:

## C Coding Standard

1. All code must be written in C.
  
2. You must use [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to format your code with the following options: 

    - Open up VSCode and type in `Ctrl + ,`
    - In the search bar, type in `clang`
    - Under the section that says **C_Cpp: Clang_format_style**, enter in

      ```
      { BasedOnStyle: LLVM, UseTab: Never, IndentWidth: 4, TabWidth: 4, ColumnLimit: 100, InsertBraces: true }
      ```
    - Then type in `Format on save` in the search bar and check the box under **Format on Save**

3. For every function you write, have a block comment before it explaining what the function does.

## Markdown Coding Guidelines

1. Headers:
- Use # for headers and follow a hierarchical structure (# for top-level, ## for second-level, etc.).

2. Lists:
- Use `-` or `*` for unordered lists.
- Use `1.` for ordered lists.

3. Formatting:
- Bold: Use `**bold text**`
- Italic: Use `*italicized text*`

4. Links:
- Inline links should be formatted as `[link text](URL)`.

5. Code Blocks:
- Indent code blocks with four spaces or use triple backticks ``` for fenced code blocks.

6. Escape Characters:
- Use backslash `\` to escape Markdown characters if needed.

7. Comments:
- Use HTML comments `<!-- comment -->` sparingly for notes or explanations within the Markdown.
