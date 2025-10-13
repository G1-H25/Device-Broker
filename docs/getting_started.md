# Getting started

## Setting up commit pre-hooks

Run:

```sh
./setup-pre-commit.sh
```

## Building and Testing the Project

### Building

To build the program and flash it to the esp32 run the following command:

```sh
pio run -e esp32
```

### Testing

There are multiple ways to test the functionality of this program. To test on the board the following command is used:

```sh
pio run -e esp32_test
```

And to test on the host device

```sh
# Unit tests
pio run -e native_unit

# Smoke tests
pio run -e native_smoke
```

**Troubleshooting**

If you encounter any issues during the build process, refer to the [PlatformIO documentation](https://docs.platformio.org/page/projectconf.html) for troubleshooting guides and tips.

By following these steps and customizing your `platformio.ini` file, you can set up PlatformIO to build and test your project efficiently.
