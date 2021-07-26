#%%
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression, SGDClassifier, Perceptron
X = pd.read_csv("dataset.csv")
y = pd.read_csv("categorical_dataset.csv", header=0, usecols=['averageRating', 'genres'])
y = y[~y['genres'].str.contains('Adult')]
y = y['genres']
y = LabelEncoder().fit_transform(y)


X_train, X_test, y_train, y_test = train_test_split(X[:100], y[:100], test_size=0.3)


clf = LinearRegression()
clf.fit(X_train, y_train)
clf.score(X_train, y_train)
clf.fit(X_test, y_test)
clf.score(X_test, y_test)



sgd = SGDClassifier()
sgd.fit(X_train, y_train)
sgd.score(X_train, y_train)
sgd.fit(X_test, y_test)
sgd.score(X_test, y_test)

pcp = Perceptron(tol=1e-3, random_state=0)
pcp.fit(X_train, y_train)
pcp.score(X_train, y_train)
pcp.fit(X_test, y_test)
pcp.score(X_test, y_test)
# %%
