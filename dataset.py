from sklearn.preprocessing import LabelEncoder
import pandas as pd

lbl_encoder = LabelEncoder()
dataset = pd.read_csv('categorical_dataset.csv')
dataset = dataset[~dataset['genres'].str.contains('Adult')]

dataset['genres'] = lbl_encoder.fit_transform(dataset['genres'])

dataset[['startYear', 'genres', 'numVotes']].to_csv('dataset.csv', index=None)