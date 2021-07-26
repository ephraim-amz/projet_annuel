#%%
import tensorflow as tf

from tensorflow.keras import layers
from tensorflow.keras.layers.experimental import preprocessing
import pandas as pd
import matplotlib.pyplot as plt


def main(lr, epochs, name):
    X = pd.read_csv('dataset.csv')
    Y = pd.read_csv('categorical_dataset.csv', usecols=['averageRating', 'genres'])
    Y = Y[~Y['genres'].str.contains('Adult')]
    Y = Y['averageRating']
    normalizer = preprocessing.Normalization(axis=-1)

    linear_model = tf.keras.Sequential([
        normalizer, 
        layers.Dense(units=1)  
    ])

    linear_model.compile(
        optimizer=tf.optimizers.Adam(learning_rate=lr),
        loss='mean_absolute_error')

    history = linear_model.fit(
        X, Y, 
        epochs=epochs,
        verbose=0,
        validation_split = 0.2)

    plt.plot(history.history['loss'], label='loss')
    plt.plot(history.history['val_loss'], label='val_loss')
    plt.xlabel('Epoch')
    plt.ylabel('Error [MPG]')
    plt.legend()
    plt.grid(True)
    plt.savefig(name)

if __name__ == '__main__':
    main(0.1, 100, 'courbe3.png')
# %%
