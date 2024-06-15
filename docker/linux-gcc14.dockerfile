FROM ubuntu:noble

RUN export DEBIAN_FRONTEND=noninteractive;                              \
    export DEBCONF_NONINTERACTIVE_SEEN=true;                            \
    echo 'tzdata tzdata/Areas select Etc' | debconf-set-selections;     \
    echo 'tzdata tzdata/Zones/Etc select UTC' | debconf-set-selections; \
    apt-get update &&                                                   \
    apt-get install -qy                                                 \
    bash                                                                \
    ninja-build                                                         \
    git                                                                 \
    curl                                                                \
    zip                                                                 \
    unzip                                                               \
    wget                                                                \
    pkg-config                                                          \
    python3                                                             \
    python3-pip                                                         \
    python-is-python3                                                   \
    software-properties-common                                          \
    gcc-14                                                              \
    g++-14                                                              \
    lld-18                                                              \
    # Install packages required by dependencies
    autoconf                                                            \
    libtool                                                             \
    xorg-dev                                                            \
    freeglut3-dev                                                       \
    libx11-dev                                                          \
    libudev-dev                                                         \
    libgl1-mesa-dev                                                     \
    libdbus-1-3                                                         \
    libgl-dev                                                           \
    libx11-xcb-dev                                                      \
    libfontenc-dev                                                      \
    libice-dev                                                          \
    libsm-dev                                                           \
    libxau-dev                                                          \
    libxaw7-dev                                                         \
    libxcomposite-dev                                                   \
    libxcursor-dev                                                      \
    libxdamage-dev                                                      \
    libxdmcp-dev                                                        \
    libxext-dev                                                         \
    libxfixes-dev                                                       \
    libxft-dev                                                          \
    libxi-dev                                                           \
    libxinerama-dev                                                     \
    libxkbfile-dev                                                      \
    libxmu-dev                                                          \
    libxmuu-dev                                                         \
    libxpm-dev                                                          \
    libxrandr-dev                                                       \
    libxrender-dev                                                      \
    libxres-dev                                                         \
    libxss-dev                                                          \
    libxt-dev                                                           \
    libxtst-dev                                                         \
    libxv-dev                                                           \
    libxvmc-dev                                                         \
    libxxf86vm-dev                                                      \
    xtrans-dev                                                          \
    libxcb-render0-dev                                                  \
    libxcb-render-util0-dev                                             \
    libxcb-xkb-dev                                                      \
    libxcb-icccm4-dev                                                   \
    libxcb-image0-dev                                                   \
    libxcb-keysyms1-dev                                                 \
    libxcb-randr0-dev                                                   \
    libxcb-shape0-dev                                                   \
    libxcb-sync-dev                                                     \
    libxcb-xfixes0-dev                                                  \
    libxcb-xinerama0-dev                                                \
    xkb-data                                                            \
    libxcb-dri3-dev                                                     \
    uuid-dev                                                            \
    libxcb-util-dev                                                     \
    xvfb &&                                                             \
    # Clean cache to keep some space.
    apt-get clean &&                                                    \
    rm -rf /var/lib/apt/lists/*

# Setup vukan sdk
RUN wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | tee /etc/apt/trusted.gpg.d/lunarg.asc &&         \
    wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list http://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list

RUN apt-get update &&           \
    apt-get install -qy         \
    vulkan-sdk &&               \
    apt-get clean &&            \
    rm -rf /var/lib/apt/lists/*

# Setup alternatives
RUN update-alternatives --install /usr/bin/lld lld /usr/bin/lld-18 10 &&            \
    update-alternatives --install /usr/bin/ld.lld ld.lld /usr/bin/ld.lld-18 10 &&   \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 10 &&            \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 10

# Install CMake
RUN pip3 install --no-cache --break-system-packages cmake

# Install Vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git && cd vcpkg && git reset --hard 8eb57355a4ffb410a2e94c07b4dca2dffbee8e50 && cd ..
RUN cd vcpkg && ./bootstrap-vcpkg.sh && ./vcpkg integrate install && cd ..

# Setup Vcpkg
ENV PATH="${PATH}:/vcpkg"
ENV VCPKG_DEFAULT_BINARY_CACHE="/vcpkg_cache"
RUN mkdir $VCPKG_DEFAULT_BINARY_CACHE

CMD ["/bin/bash"]
