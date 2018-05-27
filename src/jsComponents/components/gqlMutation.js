import React from "react";

import gql from 'graphql-tag';
import { ApolloClient } from "apollo-client";
import { InMemoryCache } from 'apollo-cache-inmemory';
import { HttpLink } from 'apollo-link-http';

export default class GqlMutation extends React.Component {
  constructor(props) {
    super(props);
    this.apolloClient = new ApolloClient({
      link: new HttpLink({ uri: 'https://q80vw8qjp.lp.gql.zone/graphql' }),
      cache: new InMemoryCache()
    });
  }
  componentDidMount() {
    this.apolloClient.query({
      query: gql`query {hello}`
    }).then(result => {
      console.log("Js didMount: ");
      console.log(result.data);
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