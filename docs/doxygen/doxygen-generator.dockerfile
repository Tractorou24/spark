FROM tracto/spark-doxygen-build-tools:latest

# --------------------------------------------
# Add code and custom documentation
# --------------------------------------------
WORKDIR /sources
ADD ./README.md ./README.md
ADD ./docs ./docs
ADD ./experimental ./experimental
ADD ./spark ./spark

# --------------------------------------------
# Generate documentation
# --------------------------------------------
WORKDIR /docs
RUN mv $DOXYGEN_AWESOME_CSS/* .
RUN mv /sources/docs/doxygen/* .
RUN doxygen Doxyfile
RUN mkdir -p ./html/docs
RUN mv /sources/docs/img ./html/docs/img
