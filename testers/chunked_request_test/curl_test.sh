#!/bin/bash
curl -H "Transfer-Encoding: chunked" -H "Content-Type: application/base64" --data-binary @$FILENAME http://localhost:6969/hering/chunked.py
