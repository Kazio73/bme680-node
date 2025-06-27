{
  "targets": [
    {
      "target_name": "bme680",
      "sources": [
        "src/bme680_wrap.c",
        "src/bme680.c",
        "src/lib/bme68x.c",
        "src/lib/bme68x_defs.c"
      ],
      "include_dirs": [
        "src",
        "src/lib"
      ],
      "cflags": [],
      "defines": [],
      "libraries": ["-lm"]
    }
  ]
}
Po zmianie