# OSC Messaging Interface

This is a simple OSC messaging overlay for `cnmat/OSC` and PlatformIO (Arduino). It allows for easy sending and receiving of OSC messages over UDP, as well as setting up and verifying a connection to another machine.

## Installation

This project is a base project and should not be used as a dependency. Instead, clone this repository and use it as a base for your own project.

## Usage

### Setup

First you need to create an instance of the `OSCHandler` class. The constructor takes three arguments: the IP address of the receiver, the port to send messages to, and the port to receive messages on. Note that the receiver ip must be a `uint8_t` array of length 4. For example:

```cpp
uint8_t RECEIVER_IP[4] = {10, 7, 50, 195};
OSCHandler oscHandler(RECEIVER_IP, 8000, 9000);
```

Then within your setup function, call the `begin` method of the `OSCHandler` instance. This method takes two arguments: the SSID of the WiFi network to connect to, and the password of the WiFi network. For example:

```cpp
oscHandler.begin(SSID, WLAN_PASSWORD);
```

> Note: The `begin` method will block until a connection is established. If the connection takes too long to establish, the ESP will restart.

### Sending Messages

To send a message, use the `sendMessage` method. The first argument is the address of the message, the second is the typed message. For example `sendMessage("/test", 1)` will send the integer `1` to the address `/test`.

This function will return `0` if the message was sent successfully, and `-1` if there was an error.

The `sendMessage` method has several overloads that allow you to send messages of different types. See [Sending Messages of Different Types](#sending-messages-of-different-types) for more information.

### Sending Messages of Different Types

The `sendMessage` method has several overloads that allow you to send messages of different types. The first argument is always the address of the message. The second argument is the message itself. The third argument is the type of the message. The following types are supported:

- `int`
- `float`
- `double`
- `const char*`
- `std::string`

If you need to add additional types, you can add them via the `SEND_TYPED_MESSAGE(type)` macro. For example, to add support for `bool` messages, you would add the following line to the `OSC.h` file:

```cpp
SEND_TYPED_MESSAGE(bool)
```

For more complex types, you can create a custom conversion method and use the `sendMessage` method that takes a `const char*` or any other existing overload as the message. To create the function header, you can use the `SEND_TYPED_MESSAGE_DECLARATION(type)` macro. For example:

```cpp
SEND_TYPED_MESSAGE_DECLARATION(std::vector<int>) {
    // Conversion code here...
    return sendMessage(address, converted_message);
}
```

> Note: The sendMessage returns an integer that is either `0` or `-1` depending on whether the message was sent successfully or not. If you are using the `SEND_TYPED_MESSAGE_DECLARATION` macro, you should return this value.

## License

You can use this code for whatever you want. If you do use it, I'd love to hear about it! Attribution is not required, but it is appreciated. :)
