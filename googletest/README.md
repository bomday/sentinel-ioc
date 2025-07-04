### Run Tests

#### Exigências
1. Rodar dentro da pasta /src
2. Ter uma versão g++ 7.0 ou superior no Windows

#### Compile o programa para rodar os testes (se necessário)
```bash
g++ -o sentinel_tests.exe utils/utils.cpp indicator/indicator.cpp maliciousIP/maliciousIP.cpp maliciousURL/maliciousURL.cpp maliciousHash/maliciousHash.cpp indicatorManager/indicatorManager.cpp fileManager/fileManager.cpp ../tests/tests_maliciousURL.cpp ../tests/tests_fileManager.cpp ../tests/tests_maliciousHash.cpp ../tests/tests_maliciousIP.cpp ../tests/tests_utils.cpp ../googletest/src/gtest-all.cc ../googletest/src/gtest_main.cc -I. -I../src/utils -Iindicator -ImaliciousIP -ImaliciousURL -ImaliciousHash -IindicatorManager -IfileManager -I../googletest/include -I../googletest -std=c++17
```
#### Execute o programa
```bash
./sentinel_tests.exe
```