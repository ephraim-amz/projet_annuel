import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression, SGDClassifier, Perceptron
from sklearn.metrics import mean_squared_error, r2_score, accuracy_score

def main():
    X = pd.read_csv("dataset.csv")
    y = pd.read_csv("categorical_dataset.csv", header=0, usecols=['averageRating', 'genres'])
    y = y[~y['genres'].str.contains('Adult')]
    y = y['averageRating']

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3)


    rgr = LinearRegression()
    rgr.fit(X_train, y_train)
    rgr.score(X_train, y_train)


    y_pred = rgr.predict(X_test)

    print('Coefficients: \n', rgr.coef_)
    print(f'Mean squared error: {mean_squared_error(y_test, y_pred):.2f}')
    print(f'Coefficient: {r2_score(y_test, y_pred):.2f}')

    plt.scatter(X_test['numVotes'], y_test,  color='black')
    plt.plot(X_test['numVotes'], y_pred, color='blue', linewidth=3)

    plt.xticks(())
    plt.yticks(())

    plt.show()


if __name__ == '__main__':
    main()