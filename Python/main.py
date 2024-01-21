import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset
import onnx
import glob

data_directory = 'data'
input_size = 3

data = []
for file_path in glob.glob(data_directory + '/*.txt'):
    with open(file_path, 'r') as file:
        for line in file:
            values = [float(x) if i != input_size else int(x) for i, x in enumerate(line.split())]
            data.append(values)
data = torch.tensor(data)

X = data[:, :input_size]
y = data[:, input_size].long()

# Создание набора данных
dataset = TensorDataset(X, y)
dataloader = DataLoader(dataset, batch_size=16, shuffle=True)


class SimpleNN(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(SimpleNN, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(hidden_size, output_size)
        self.softmax = nn.LogSoftmax(dim=1)

    def forward(self, x):
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        x = self.softmax(x)
        return x


hidden_size = 128
output_size = len(torch.unique(y))
model = SimpleNN(input_size, hidden_size, output_size)

criterion = nn.NLLLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

num_epochs = 5
for epoch in range(num_epochs):
    for inputs, labels in dataloader:
        optimizer.zero_grad()
        output = model(inputs)
        loss = criterion(output, labels)
        loss.backward()
        optimizer.step()
    print(f'Epoch {epoch + 1}, Loss: {loss.item()}')

dummy_input = torch.randn(1, input_size)
dummy_output = model(dummy_input)
input_names = ["combined_input"]
output_names = ["actor_action"]
model_path = 'model.onnx'
torch.onnx.export(
    model,
    dummy_input,
    model_path,
    verbose=False,
    input_names=input_names,
    output_names=output_names,
    export_params=True
)
