# Tests

This directory contains the unit tests for the project, implemented using **GoogleTest**.
Below are the key patterns and concepts used in this test suite.

### 1. AAA Principle (Arrange, Act, Assert)
Every test follows the **Arrange-Act-Assert** pattern to ensure clarity and separation of concerns.

### 2. Test Fixtures for Shared Setup
Each component has its own test fixture class that inherits from `::testing::Test` for shared setup and teardown logic.

### 3. Tests are Typed where Appropriate
For components that need to be tested with multiple data types, typed tests are used to ensure
coverage across different scenarios.

### 4. Single Responsibility per Test
Each test case is designed to verify a single behavior or functionality of the component under test.