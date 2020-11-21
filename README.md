# npipe

### Simple named pipe server for Windows

*For those pesky programs that don't have an option to write to standard out*

## Usage

```
C:\somepath> npipe foo | process-data
C:\somepath> transmogrify-to-file -in exports.csv -out \\.\pipe\foo
```

## Download

Please see the releases section for x64 Windows binaries.

To build, run:

```
build
```

(or `build STATIC=1` to statically link the C runtime library)

## License

[ISC License](https://choosealicense.com/licenses/isc/)

## Why I needed this

Transcoding video files on my Xbox One. Long story.