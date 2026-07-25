FROM ubuntu:24.04

# Install build tools
RUN apt-get update && \
    apt-get install -y \
        cmake \
        g++ \
        make

# Copy project
WORKDIR /app

COPY . .

# Build
RUN cmake -B build
RUN cmake --build build

# Expose MiniRedis port
EXPOSE 6379

# Start server
CMD ["./build/MiniRedis"]