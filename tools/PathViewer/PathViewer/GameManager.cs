using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;

namespace PathViewer
{
    public class GameManager
    {
        #region private varaibles
        private IDictionary<string, Game> m_games;
        #endregion

        #region public constructor
        public GameManager()
        {
            m_games = new Dictionary<string, Game>();
            ReadGames();
        }
        #endregion

        #region public properties
        public IEnumerable<Game> Games
        {
            get { return m_games.Values; }
        }

        public IEnumerable<string> GameNames
        {
            get { return m_games.Keys; }
        }
        
        public Game this[string name]
        {
            get { return m_games[name]; }
        }

        public int Count
        {
            get { return m_games.Count; }
        }
        #endregion

        #region public methods

        public void Clear()
        {
            m_games.Clear();
        }

        public void ReadGames()
        {
            string dir = AppDomain.CurrentDomain.BaseDirectory;

            foreach (string file in Directory.GetFiles(dir, "*.json"))
            {
                ReadGame(file);
            }
        }

        public void ReadGame(string filename)
        {
            string json = File.ReadAllText(filename);
            Game g = JsonConvert.DeserializeObject<Game>(json);
            g.Path = Path.GetDirectoryName(filename);
            m_games[g.Name] = g;
        }
        #endregion
    }
}
