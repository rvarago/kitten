FROM conanio/gcc8:1.14.1

USER root

WORKDIR kitten

COPY . .

CMD ["/bin/bash"]
