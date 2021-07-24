import pandas as pd
import numpy as np
from random import randint
from flask import Flask, render_template, request

app = Flask(__name__)
data = pd.read_csv('categorical_dataset.csv')
if 'user_note' not in data.columns:
    data['user_note'] = None


@app.route('/', methods=['GET', 'POST'])
def home():
    return render_template('home.html')

def load_random_movie():
    random_movie = data.iloc[randint(0, len(data)), :]
    return random_movie

@app.route('/movie', methods=['GET'])
def movie():
    global random_movie 
    random_movie = load_random_movie()
    title = random_movie['originalTitle']
    director = random_movie['director']
    primary_name = random_movie['primaryName']
    genres = random_movie['genres']
    annee_sortie = random_movie['startYear']
    average_rating = random_movie['averageRating']

    return render_template('movie.html', title=title, director=director, primary_name=primary_name, genres=genres, annee_sortie=annee_sortie, average_rating=average_rating)

@app.route('/note', methods=['POST'])
def note():
    note_user = float(format(float(request.form['note']), ".2f"))
    title = random_movie['originalTitle']
    director = random_movie['director']
    primary_name = random_movie['primaryName']
    genres = random_movie['genres']
    average_rating = random_movie['averageRating']
    annee_sortie = random_movie['startYear']
    if note_user > average_rating:
        diff = note_user-average_rating
    else:
        diff = average_rating-note_user

    diff = float(format(float(diff), ".2f"))
    
    
    return render_template('note.html', note_user=note_user, note_film=average_rating, title=title, director=director, primary_name=primary_name, genres=genres, annee_sortie=annee_sortie, hasNote=True, diff=diff)  

@app.route('/no_note', methods=['GET','POST'])
def note2():
    note_user = request.form['no_note']

    title = random_movie['originalTitle']
    director = random_movie['director']
    primary_name = random_movie['primaryName']
    genres = random_movie['genres']
    average_rating = random_movie['averageRating']
    annee_sortie = random_movie['startYear'] 


    return render_template('note.html', note_user=note_user, note_film=average_rating, title=title, director=director, primary_name=primary_name, genres=genres, annee_sortie=annee_sortie, hasNote=False)  


if __name__ == '__main__':
    app.run(debug=True)