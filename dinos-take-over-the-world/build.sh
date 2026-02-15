#!/bin/bash

docker build -f Dockerfile --target exporter --output type=local,dest=. .
