rer
===

regular expression based file renamer

rer is a tool that rename files that matches given pattern.
Each file name given as argument is matched against pattern.
If a match is found file is renamed according to given replacement using PCRE regular expression.

## Usage

    rer /pattern/replacement/modifiers file [file [...]]

## Examples

rename foobar to foobaz

    $ rer /bar/baz/ foobar

Swap first word with second, renaming foo-bar to bar-foo, foo-baz to baz-foo and foo-123 to 123-foo

    $ rer '/(\w+)-(\w+)/\2-\1/' foo-bar foo-baz foo-123
