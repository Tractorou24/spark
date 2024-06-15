FROM ubuntu:noble

RUN export DEBIAN_FRONTEND=noninteractive;                              \
    export DEBCONF_NONINTERACTIVE_SEEN=true;                            \
    echo 'tzdata tzdata/Areas select Etc' | debconf-set-selections;     \
    echo 'tzdata tzdata/Zones/Etc select UTC' | debconf-set-selections; \
    apt-get update &&                                                   \
    apt-get install -qy                                                 \
    bash                                                                \
    git                                                                 \
    python3                                                             \
    python3-pip                                                         \
    python-is-python3 &&                                                \
    # Clean cache to keep some space.
    apt-get clean &&                                                    \
    rm -rf /var/lib/apt/lists/*

# Install python packages
RUN pip install black ruff --break-system-packages

CMD ["/bin/bash"]
