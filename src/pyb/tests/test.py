import pythonBindings

def test_main():
    assert pythonBindings.__version__ == "0.0.1"
    assert pythonBindings.add(1, 2) == 3
    assert pythonBindings.subtract(1, 2) == -1

print(pythonBindings.add(1,2))
print(pythonBindings.multiply(2, 2.5))