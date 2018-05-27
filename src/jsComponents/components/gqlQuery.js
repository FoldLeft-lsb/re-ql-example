import React from "react";

import gql from 'graphql-tag';
import { ApolloClient } from "apollo-client";
import { InMemoryCache } from 'apollo-cache-inmemory';
import { HttpLink } from 'apollo-link-http';

const githubKey = require('../../config').github_key;

export default class GqlQuery extends React.Component {
  constructor(props) {
    super(props);
    this.apolloClient = new ApolloClient({
      link: new HttpLink({
        uri: 'https://api.github.com/graphql', headers: {
          "Authorization": `bearer ${githubKey}`,
          "Accept": "application/vnd.github.v3+json"
        }
      }),
      cache: new InMemoryCache()
    });
  }
  componentDidMount() {
    this.apolloClient.query({
      query: gql`
      query {
        viewer {
          name
           repositories(last: 3) {
             nodes {
               name
             }
           }
         }
      }
      `
    }).then(result => {
      console.log("Js didMount: ");
      console.log(result.data.viewer.name);
    });
  }
  render() {
    return (
      <div>
        <h1>Js Component</h1>
      </div>
    );
  }
};