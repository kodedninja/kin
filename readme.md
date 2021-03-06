# kin
A small tool to create html documents from Kindle highlights

## Usage
```
$ kin "My Highlights.txt" > highlights.html
```

## About
kin turns the Kindle highlight file into a single-file html document so you can use your browser to read it (and search it). 

I didn't like that there was no small and simple (therefore C) implementation of Kindle export scripts. To keep the code simple, the highlights are reversed with `flex-direction: column-reverse` (I'm curious if someone has a better idea here) so if your browser doesn't support `flex`, you'll see them in chronological order.

Inspired by [doq](https://github.com/rxi/doq) and also used the basic implementation from there.

## License
```
Copyright (c) 2020 kodedninja

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
