#!/bin/bash
source .venv/bin/activate && pyocd gdbserver --persist --target rp2040
