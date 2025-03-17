# Contributing to ESP32-WebCommunication

Thank you for considering contributing to the ESP32-WebCommunication library! This document outlines the process for contributing to the project.

## Code of Conduct

Please be respectful and considerate of others when participating in this project. We aim to foster an inclusive and welcoming community.

## How to Contribute

### Reporting Bugs

If you find a bug, please open an issue on the GitHub repository with the following information:

1. A clear, descriptive title
2. A detailed description of the issue
3. Steps to reproduce the bug
4. Expected behavior
5. Actual behavior
6. Environment details (ESP32 board model, Arduino IDE version, etc.)
7. Any relevant logs or screenshots

### Suggesting Features

We welcome feature suggestions! Please open an issue with:

1. A clear, descriptive title
2. A detailed description of the proposed feature
3. Any relevant examples or use cases
4. Why this feature would be beneficial

### Pull Requests

We welcome pull requests for bug fixes, features, and improvements. Please follow these steps:

1. Fork the repository
2. Create a new branch: `git checkout -b feature/your-feature-name`
3. Make your changes
4. Run tests: `pio test -e test`
5. Commit your changes with descriptive commit messages
6. Push to your fork: `git push origin feature/your-feature-name`
7. Submit a pull request

### Coding Standards

Please follow these coding standards when contributing:

1. Use consistent indentation (2 spaces)
2. Use descriptive variable and function names
3. Add comments to explain complex code sections
4. Follow Arduino library best practices
5. Keep memory usage minimal - the ESP32 has limited resources

### Testing

All new features and bug fixes should include appropriate tests. To run tests:

```
pio test -e test
```

## Development Setup

To set up the development environment:

1. Install [PlatformIO](https://platformio.org/)
2. Clone the repository: `git clone https://github.com/AcierDev/ESP32-WebCommunication.git`
3. Open the project in PlatformIO
4. Install dependencies

## License

By contributing to this project, you agree that your contributions will be licensed under the MIT License.
