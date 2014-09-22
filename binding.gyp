{
  "targets": [
    {
       "cflags": [
        "-g",
	"-I/usr/local/include/tcl8.6",
        "-D_FILE_OFFSET_BITS=64",
        "-D_LARGEFILE_SOURCE",
        "-Wall"
      ],
      "ldflags": [
        "-L/usr/local/lib",
        "-ltcl86"
      ],
      "sources": [
        "src/nodetcl.cc"
      ],
      "target_name": "nodetcl"
    }
  ]
}
