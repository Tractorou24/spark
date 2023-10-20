FROM alpine:latest

# --------------------------------------------
# Install needed packages
# --------------------------------------------
RUN apk add --no-cache  \
    git                 \
    make                \
    doxygen

# --------------------------------------------
# Install doxygen theme
# --------------------------------------------
WORKDIR /doxygen-theme
RUN git clone https://github.com/jothepro/doxygen-awesome-css.git --branch v2.2.1 --depth 1 .
RUN make install

ENV DOXYGEN_AWESOME_CSS=/usr/local/share/doxygen-awesome-css

# Detete cloned sources after install to /usr/local/share/doxygen-awesome-css
WORKDIR /
RUN rm -rf /doxygen-theme
