# Use an official Ubuntu as a parent image
FROM ubuntu:20.04

# Set environment variables to non-interactive to avoid prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages and libraries
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    libmariadb-dev \
    git \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Install Paho MQTT C library
RUN git clone https://github.com/eclipse/paho.mqtt.c.git /tmp/paho.mqtt.c \
    && cd /tmp/paho.mqtt.c \
    && git checkout v1.3.13 \
    && cmake -Bbuild -H. -DPAHO_WITH_SSL=ON \
    && cmake --build build/ --target install \
    && cd / \
    && rm -rf /tmp/paho.mqtt.c

# Install Paho MQTT C++ library
RUN git clone https://github.com/eclipse/paho.mqtt.cpp.git /tmp/paho.mqtt.cpp \
    && cd /tmp/paho.mqtt.cpp \
    && git checkout v1.4.0 \
    && cmake -Bbuild -H. -DPAHO_WITH_SSL=ON \
    && cmake --build build/ --target install \
    && cd / \
    && rm -rf /tmp/paho.mqtt.cpp

# Update the library cache
RUN ldconfig

# Set working directory
WORKDIR /app

# Copy source code to the container
COPY . /app

# Build the project
RUN g++ -o main main.cpp -lpaho-mqttpp3 -lpaho-mqtt3as -lmariadb -pthread

# Command to run the application
CMD ["./main"]
