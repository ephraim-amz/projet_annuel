import pandas as pd

separators = "\t+|\t+\s+"
movie_columns = ['tconst', 'titleType', 'originalTitle', 'primaryTitle', 'startYear', 'runtimeMinutes', 'genres']
df_movie = pd.read_csv('./data/title.basics.tsv/data.tsv', sep=separators, header=0, usecols=movie_columns,
                       encoding='utf-8', engine='python')
df_movie['runtimeMinutes'] = pd.to_numeric(df_movie['runtimeMinutes'], errors='coerce')
df_movie['AIRating'] = None
df_movie['PrimaryName'] = None

df_ratings = pd.read_csv('./data/title.ratings.tsv/data.tsv', header=0,
                         sep=separators, encoding='utf-8', engine='python')
df_actors = pd.read_csv('./data/name.basics.tsv/data.tsv', header=0,
                        sep=separators, encoding='utf-8', engine='python')
df_actors['deathYear'] = pd.to_numeric(df_actors['deathYear'], errors='coerce')

df_actors['knownForTitles'] = df_actors['knownForTitles'].str.split(',')

df_actors['deathYear'] = pd.to_numeric(df_actors['deathYear'], errors='coerce')
df_credits = pd.read_csv('./data/title.principals.tsv/data.tsv', header=0,
                         sep=separators, encoding='utf-8', engine='python')

df_movie_ratings = pd.merge(df_movie, df_ratings, how='right', on='tconst')
del df_ratings, df_movie

# df_actors_credits = pd.merge(df_actors, df_credits, how='str', on=str)
df_movie_ratings[df_movie_ratings.tconst.isin(df_actors['knownForTitles'])]