# PSBBV architecture

## Current implementation boundary

The first native milestone is a small C++17 navigation core shared by the Vita application and host regression tests.

- psbbv_navigation owns the six stable channel identifiers and the menu/open/back state machine.
- src/main.cpp is the VitaSDK entry point. It handles the Vita controller, draws the 960 × 544 interface with libvita2d and displays an honest placeholder page for channels whose domain modules are not connected.
- Host builds compile and test the portable navigation core without requiring VitaSDK.
- Vita builds use the VitaSDK toolchain, libvita2d and system controller/kernel stubs to produce PSBBV.vpk.

## Ownership boundaries

PSBBV Core owns navigation, rendering, application lifecycle and node orchestration. Federated repositories own their domain logic:

| Node | Owns |
|---|---|
| Games | Installed-title discovery, metadata and library |
| Applications | Homebrew/application inventory and management |
| Media | Media catalogue, providers and playback |
| Network | Connectivity state and secure network adapters |
| Downloads | Transfer queue, staging and integrity |
| System | Preferences, diagnostics and safe system tools |

A node being listed in the product map does not mean its feature is implemented in this initial shell.

## Input contract

- D-pad Up/Down moves the selected channel and wraps at the ends.
- Cross opens the selected channel.
- Circle returns from a channel placeholder to the main menu.
- Start exits the application.
- Input is edge-triggered so a held button does not repeat an action every frame.

## Safety and product constraints

- No commercial game data, keys, credentials or copyrighted payloads are included.
- Unimplemented channels are explicitly labelled as placeholders.
- The initial shell is offline-capable and does not issue network requests.
- The application does not modify system settings or installed content.
