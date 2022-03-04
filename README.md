# search-open

A small utility that acts as a quick search on your system.

![search-open Preview](/Screenshot.png)

## Why?

For a couple reasons:

- It makes for an excellent project for me to learn GTK and apply C++
- I rely heavily on an easily accessible menu with a search bar, and having a dead simple program that does so is really nice for me

## Installation

You should be good to `git clone` the repository and `make` it! You'll need GTK3, though.

## Keeping it Updated

I don't currently have an apt repository set up, nothing in the AUR, etc. Until any of those exist (if they ever do), feel free to use one of the methods below to keep this up to date.

### tarstall

To shamelessly plug my other project, you can use [tarstall](https://github.com/hammy3502/tarstall) to keep it up to date! `tarstall -i https://github.com/hammy3502/search-open.git`, then you can set the post-upgrade script to the `build.sh` script, and you'll be good to go!

### Without a Package Manager

`git pull` and `./build.sh` will update your copy of `search-open`
