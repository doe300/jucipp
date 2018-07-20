# Setup of tested language servers

## JavaScript with Flow static type checker
* Prerequisites:
    * Node.js
* Recommended:
    * [Prettier](https://github.com/prettier/prettier)

Install language server, and create executable to enable server in juCi++:
```sh
npm install -g flow-language-server

echo '#!/bin/bash
flow-language-server --stdio' > /usr/local/bin/javascript-language-server

chmod 755 /usr/local/bin/javascript-language-server
```

* Additional setup within a JavaScript project:
    * Add a `.prettierrc` file to enable style format on save

## Python3
* Prerequisites:
    * Python3
    * In juCi++ preferences, set `project.python_command` to `PYTHONUNBUFFERED=1 python3`
  
Install language server, and create symbolic link to enable server in juCi++:
```sh
pip3 install python-language-server[rope,yapf]

ln -s pyls /usr/local/bin/python-language-server
```

* Additional setup within a Python project:
    * Add a setup file, for instance: `printf '[pycodestyle]\nmax-line-length = 120\n\n[yapf]\nCOLUMN_LIMIT = 120\n' > setup.cfg`
    * Add an empty `.python-format` file to enable style format on save

## Rust
* Prerequisites:
    * Rust
      
Install language server, and create symbolic link to enable server in juCi++:
```sh
rustup update
rustup component add rls-preview rust-analysis rust-src

ln -s `rustc --print sysroot`/bin/rls /usr/local/bin/rust-language-server
```

* Additional setup within a Rust project:
    * Add an empty `.rust-format` file to enable style format on save
