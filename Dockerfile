FROM conanio/gcc9:1.19.0

USER root

WORKDIR kitten

COPY . .

CMD ["/bin/bash"]
