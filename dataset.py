from sklearn.preprocessing import LabelEncoder
import pandas as pd
dataset = pd.read_csv('dataset.csv')

label_enc = LabelEncoder()

for col in dataset.columns:
    if col not in ['startYear','runtimeMinutes','averageRating','numVotes']:
        dataset[col] = label_enc.fit_transform(dataset[col])

dataset['startYear'] = dataset['startYear'].astype(int)
dataset = dataset.sort_values(by='startYear', ascending=False)

dataset.to_csv('label_encoded_dataset.csv')
