import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset
import onnx

# Предполагаем, что ваш файл с данными называется 'data.txt'
data_path = 'data.txt'

# Чтение данных и преобразование в тензоры Torch
data = []
with open(data_path, 'r') as file:
    for line in file:
        values = [float(x) if i != 3 else int(x) for i, x in enumerate(line.split())]
        data.append(values)
data = torch.tensor(data)

# Разделение данных на входы и метки
X = data[:, :3]  # Первые три числа в каждой строке - входы
y = data[:, 3].long()  # Четвертое число - метка (используем .long() для конвертации в тензор типа long)

# Создание набора данных
dataset = TensorDataset(X, y)
dataloader = DataLoader(dataset, batch_size=16, shuffle=True)


# Определение архитектуры нейросети
class SimpleNN(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(SimpleNN, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(hidden_size, output_size)
        self.softmax = nn.LogSoftmax(dim=1)  # Если задача классификации

    def forward(self, x):
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        x = self.softmax(x)
        return x


# Инициализация нейросети
input_size = 3
hidden_size = 128
output_size = len(torch.unique(y))  # Количество уникальных действий
model = SimpleNN(input_size, hidden_size, output_size)

# Определение функции потерь и оптимизатора
criterion = nn.NLLLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

# Обучение нейросети
num_epochs = 5
for epoch in range(num_epochs):
    for inputs, labels in dataloader:
        optimizer.zero_grad()
        output = model(inputs)
        loss = criterion(output, labels)
        loss.backward()
        optimizer.step()
    print(f'Epoch {epoch + 1}, Loss: {loss.item()}')

# Экспорт обученной модели в формат ONNX
dummy_input = torch.randn(1, input_size)  # Пример входа для тестирования экспорта
model_path = 'model.onnx'  # Путь для сохранения модели
torch.onnx.export(model, dummy_input, model_path)

print(f'Model has been exported to {model_path}') 