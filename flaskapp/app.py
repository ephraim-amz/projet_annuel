import pandas as pd
from random import randint
from flask import Flask, render_template, request

app = Flask(__name__)
data = pd.read_csv('../dataset.csv')


@app.route('/', methods=['GET', 'POST'])
def home():
    return render_template('home.html')

def load_random_movie():
    random_movie = data.iloc[randint(0, len(data)), :]
    return random_movie


@app.route('/movie', methods=['GET'])
def movie():
    random_movie = load_random_movie()
    title = random_movie['originalTitle']
    director = random_movie['director']
    primary_name = random_movie['primaryName']
    genres = random_movie['genres']
    average_rating = random_movie['averageRating']
    # elements = (title, director, primary_name, genres, average_rating)
    # Augmenter la variable numVotes lorsque l'utilisateur note un film
    return render_template('movie.html', title=title, director=director, primary_name=primary_name, genres=genres, average_rating=average_rating)

@app.route('/note', methods=['POST'])
def note():
    note = request.form['chiffre']
    print(note)
    return render_template('note.html', note=note)  


if __name__ == '__main__':
    app.run(debug=True)
