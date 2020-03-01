FROM conanio/gcc9:1.22.2

RUN pip install clang-format

USER root

WORKDIR kitten

COPY . .

CMD ["/bin/bash"]
