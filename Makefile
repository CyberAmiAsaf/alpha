pwd := $(shell pwd)

build-docker-dev:
	docker build . -t "alpha-dev"

run-docker-dev:
	docker run -v $(pwd):/root/alpha -i -t "alpha-dev" /bin/bash
